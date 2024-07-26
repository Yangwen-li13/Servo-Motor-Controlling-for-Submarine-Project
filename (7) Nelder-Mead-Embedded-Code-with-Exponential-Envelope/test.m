function test(arduino)
    global freqX aX g_message freq_counter a_counter a_matrix freq_matrix;
    writeline(arduino, 'test');
    freqX = freq_matrix(1 ,freq_counter);
    aX = a_matrix(1, a_counter);
    
    g_message = 'amp_400_per_100_freq_' + string(freqX) + '_a_' + string(aX);
    
    if freq_counter == size(freq_matrix, 2)
        if a_counter == size(a_matrix, 2)
            return
        end
       a_counter = a_counter + 1;
       freq_counter = 0;
    end
    freq_counter = freq_counter + 1;
end