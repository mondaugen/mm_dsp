#include "mm_common_calcs.h" 

static const float __attribute__((section(".rodata"))) et12_table[] = 
{
    0.0185813612f,
    0.0196862664f,
    0.0208568727f,
    0.0220970869f,
    0.0234110481f,
    0.0248031414f,
    0.0262780130f,
    0.0278405849f,
    0.0294960723f,
    0.0312500000f,
    0.0331082217f,
    0.0350769390f,
    0.0371627223f,
    0.0393725328f,
    0.0417137454f,
    0.0441941738f,
    0.0468220962f,
    0.0496062829f,
    0.0525560260f,
    0.0556811699f,
    0.0589921445f,
    0.0625000000f,
    0.0662164434f,
    0.0701538780f,
    0.0743254447f,
    0.0787450656f,
    0.0834274909f,
    0.0883883476f,
    0.0936441923f,
    0.0992125657f,
    0.1051120519f,
    0.1113623398f,
    0.1179842891f,
    0.1250000000f,
    0.1324328868f,
    0.1403077560f,
    0.1486508894f,
    0.1574901312f,
    0.1668549818f,
    0.1767766953f,
    0.1872883846f,
    0.1984251315f,
    0.2102241038f,
    0.2227246795f,
    0.2359685782f,
    0.2500000000f,
    0.2648657736f,
    0.2806155121f,
    0.2973017788f,
    0.3149802625f,
    0.3337099635f,
    0.3535533906f,
    0.3745767692f,
    0.3968502630f,
    0.4204482076f,
    0.4454493591f,
    0.4719371563f,
    0.5000000000f,
    0.5297315472f,
    0.5612310242f,
    0.5946035575f,
    0.6299605249f,
    0.6674199271f,
    0.7071067812f,
    0.7491535384f,
    0.7937005260f,
    0.8408964153f,
    0.8908987181f,
    0.9438743127f,
    1.0000000000f,
    1.0594630944f,
    1.1224620483f,
    1.1892071150f,
    1.2599210499f,
    1.3348398542f,
    1.4142135624f,
    1.4983070769f,
    1.5874010520f,
    1.6817928305f,
    1.7817974363f,
    1.8877486254f,
    2.0000000000f,
    2.1189261887f,
    2.2449240966f,
    2.3784142300f,
    2.5198420998f,
    2.6696797083f,
    2.8284271247f,
    2.9966141538f,
    3.1748021039f,
    3.3635856610f,
    3.5635948726f,
    3.7754972507f,
    4.0000000000f,
    4.2378523774f,
    4.4898481932f,
    4.7568284600f,
    5.0396841996f,
    5.3393594167f,
    5.6568542495f,
    5.9932283075f,
    6.3496042079f,
    6.7271713220f,
    7.1271897451f,
    7.5509945015f,
    8.0000000000f,
    8.4757047549f,
    8.9796963865f,
    9.5136569200f,
    10.0793683992f,
    10.6787188334f,
    11.3137084990f,
    11.9864566150f,
    12.6992084157f,
    13.4543426441f,
    14.2543794902f,
    15.1019890029f,
    16.0000000000f,
    16.9514095097f,
    17.9593927729f,
    19.0273138400f,
    20.1587367983f,
    21.3574376667f,
    22.6274169980f,
    23.9729132300f,
    25.3984168315f,
    26.9086852881f,
    28.5087589805f
};

/* Looks up the value 2^((pitch-69)/12). Uses linear interpolation between
 * integer-valued pitches. If pitch beyond bounds of the table, returns maximum
 * or minimum value in table. */
float MMCC_et12_rate(float pitch)
{
    if (pitch < 0) {
        return et12_table[0];
    }
    if (pitch >= (MMCC_ET12_TABLE_LENGTH - 1)) {
        return et12_table[MMCC_ET12_TABLE_LENGTH - 1];
    }
    float y0, y1, mu;
    int idx_ = (int)pitch;
    y0 = et12_table[idx_];
    y1 = et12_table[idx_+1];
    mu = pitch - idx_;
    return y0 + mu*(y1 - y0);
}
