# Passo a passo para utilizar o MSP430 no Linux

1*.* Instale as dependências:
``` Shell
sudo apt-get install msp430-libc mspdebug msp430mcu binutils-msp430 gcc-msp430 gdb-msp430
```

2*.* Compile o código fonte desenvolvido:
``` Shell
msp430-gcc -Os -mmcu=msp430g2553 -o prog.elf FILENAME
```

3*.* Escreva o código na LaunchPad:
``` Shell
sudo mspdebug rf2500
```

4*.* Programe o microcontrolador:
``` Shell
prog prog.elf
```

5*.* Execute a aplicação:
``` Shell
run
```
ou
``` Shell
Ctrl+D
```

### Referências
[1] [Trabalhando com o MSP430 no Linux](http://sergioprado.org/trabalhando-com-o-msp430-no-linux/), acesso dia 19 de outubro de 2016.
