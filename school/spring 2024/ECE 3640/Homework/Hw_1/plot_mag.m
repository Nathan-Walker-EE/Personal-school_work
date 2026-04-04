function y = plot_mag(x, F, num)

    xmag = abs(x);
    xlog = 20.*log10(abs(xmag));
    fig = subplot(2, 1, 1);
    plot(F,xmag, 'Linewidth', .2);
    str = "Magnitude P" + num;
    title(str);
    xlabel("F");
    ylabel("|X(F)|");
    subplot(2, 1, 2);
    plot(F,xlog, 'Linewidth', .2);
    str = "20log|X(F)| P" + num;
    title(str);
    xlabel("F");
    ylabel("20log|X(F)|");
    ylim([-40 20]);
    str = "Problem_" + num + ".png";
    saveas(fig, str)
    
end