#version 130

float gu[];
float g4[4];
float g5[5];

uniform int a;

float[4] foo(float a[5])
{
    return float[](a[0], a[1], a[2], a[3]);
}

void bar(float[5]) {}

void main()
{
    {
        float gu[2];  // okay, new scope

        gu[2] = 4.0;  // ERROR, overflow
    }

    gu[2] = 4.0; // okay

    gu[3] = 3.0;
    gu[a] = 5.0; // ERROR

    g4 = foo(g5);
    g5 = g4;  // ERROR
    gu = g4;  // ERROR

    foo(gu);  // ERROR
    bar(g5);

    if (float[4](1.0, 2.0, 3.0, 4.0) == g4)
        gu[0] = 2.0;

    float u[];
    u[2] = 3.0; // okay
    float u[5];
    u[5] = 5.0; // ERROR
    foo(u);     // okay

    gl_FragData[1000] = vec4(1.0); // ERROR
    gl_FragData[-1] = vec4(1.0);   // ERROR
    gl_FragData[3] = vec4(1.0);

    const int ca[] = int[](3, 2);
    int sum = ca[0];
    sum += ca[1];
    sum += ca[2];  // ERROR

    const int ca3[3] = int[](3, 2);  // ERROR
    int ica[] = int[](3, 2);
    int ica3[3] = int[](3, 2);       // ERROR
    ica[3.1] = 3;                    // ERROR
    ica[u[1]] = 4;                   // ERROR
}

int[] foo213234();        // ERROR
int foo234234(float[]);   // ERROR
int foo234235(vec2[] v);  // ERROR
