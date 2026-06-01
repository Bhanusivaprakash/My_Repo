import numpy as np

# ============================================
# PARAMETERS
# ============================================

NUM_TAPS = 16
fc = 1000
fs = 8000

# ============================================
# IDEAL FIR (FLOAT)
# ============================================

M = (NUM_TAPS - 1) / 2
n = np.arange(NUM_TAPS)

h = 2 * (fc / fs) * np.sinc(2 * (fc / fs) * (n - M))

window = np.hamming(NUM_TAPS)
h = h * window

# Normalize to unity DC gain
h = h / np.sum(h)

# ============================================
# FIXED-POINT CONVERSION (TRUE INT8 DESIGN)
# ============================================

scale = 127
h_fixed = np.round(h * scale)

# enforce int8 range
h_fixed = np.clip(h_fixed, -128, 127).astype(np.int8)

# ============================================
# RE-NORMALIZE AFTER QUANTIZATION (CRITICAL FIX)
# ============================================

gain = np.sum(h_fixed)

# avoid divide-by-zero safety (very rare but correct practice)
if gain != 0:
    h_fixed = np.round((h_fixed / gain) * 127).astype(np.int8)

# ============================================
# WRITE MEMORY FILE (CORRECT 8-bit)
# ============================================

with open("fir_coeff.mem", "w") as f:
    for val in h_fixed:
        v = int(val) & 0xFF   # convert first, then mask
        f.write(format(v, '08b') + "\n")

# ============================================
# DEBUG
# ============================================

print("Floating FIR:")
print(h)

print("\n8-bit FIR coeffs:")
print(h_fixed)

print("\nSum:", np.sum(h_fixed))