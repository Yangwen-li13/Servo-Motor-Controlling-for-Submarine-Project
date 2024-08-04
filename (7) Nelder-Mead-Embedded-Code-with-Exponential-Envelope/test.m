function test(arduino)
    global freqX aX g_message freq_counter a_counter a_matrix freq_matrix;
    writeline(arduino, 'test');
    freqX = freq_matrix(1 ,freq_counter);
    aX = a_matrix(1, a_counter);
    
    g_message = 'amp_400_per_5_freq_' + string(freqX) + '_a_' + string(aX) + '_startAmp_0';
    writeline(arduino, g_message);
end