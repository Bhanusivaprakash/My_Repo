#include <iostream>
#include <vector>

using namespace std;

int main(){
    int i = 0;
    int x2[3], y2[3], z2[3];

    // AoS
    struct Particle{
        int x;
        int y;
        int z;
    };

    Particle particles[3];

    particles[0].x = 10;
    particles[0].y = 20;
    particles[0].z = 30;

    particles[1].x = 40;
    particles[1].y = 50;
    particles[1].z = 60;

    particles[2].x = 70;
    particles[2].y = 80;
    particles[2].z = 90;

    for(i = 0; i < 3; i++){
        x2[i] = particles[i].x;
        y2[i] = particles[i].y;
        z2[i] = particles[i].z;

        cout << x2[i] << ' ';
        cout << y2[i] << ' ';
        cout << z2[i] << ' ';
    }
}