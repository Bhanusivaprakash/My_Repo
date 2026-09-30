#include <iostream>
#include <vector>

using namespace std;

int main(){
    int i = 0;

    // SoA
    struct Particle{
        int x[3];
        int y[3];
        int z[3];
    };

    // AoS
    struct Particle2{
        int x2;
        int y2;
        int z2;
    };

    Particle particles;
    Particle2 particles2[3];

    particles.x[0] = 10; particles.x[1] = 40; particles.x[2] = 70;
    particles.y[0] = 20; particles.y[1] = 50; particles.y[2] = 80;
    particles.z[0] = 30; particles.z[1] = 60; particles.z[2] = 90;

    for(i = 0; i < 3; i++){
        particles2[i].x2 = particles.x[i];
        particles2[i].y2 = particles.y[i];
        particles2[i].z2 = particles.z[i];

        cout << particles2[i].y2 << ' ';
    }


}