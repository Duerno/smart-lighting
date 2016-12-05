set key

if(!exists('input')) input = 'measures.dat'

f(x) = a + b / x
fit f(x) input via a,b

plot input lc rgb 'blue' title 'Valores Medidos', \
           f(x) lc rgb 'red' title 'Curva ajustada'

# set logscale x
# set logscale y
set xlabel 'Lux [mA]'
set ylabel 'R_{LDR} [Ohms]'
set title 'Grafico de resistencia por luminosidade no LDR'
set grid

set terminal postscript eps enhanced 'Helvetica' 14

set output 'chart.eps'

replot
