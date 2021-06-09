产生波形流程：
g_Sine.Frequency = 2000; // 频率为2000
g_Sine.Amplitude = 2048; // 幅值为（2048/4095+2047/4095）*3.3=3.3
InitWaveMaker(); // 初始化
SetupWaveType(WT_SIN, g_Sine.Frequency, g_Sine.Amplitude, 0); // 产生波形

当然可以直接调用rect()函数或者其他波形的参数，修改参数即可。