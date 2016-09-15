# FPGA Display (IP Driver)

This is the C Driver library used in pair with [FPGADisplay-ipcore](https://github.com/zxcmehran/FPGADisplay-ipcore). Its required to install the driver in order to programmatically control display output.

It lets the designer to control each pixel on the screen. The designer will be able to plot basic geometric shapes including lines, triangles, rectangles and circles. It won't use any floating point operations and the whole system is implemented using integers.

It also lets the designer to print character strings over the screen using a monospace typeface. In addition, it provides a terminal-like area on the screen which reflows the text and can be useful for debugging and output monitoring.

TODO: Add documentation.

## Installing
This repository contains **IP Driver files**. Clone it in a directory named `display_handler_v1_00_a` under `drivers` directory in your project root or in global user peripheral repository of your EDK installation.

    $ cd [project dir or global repo]
    $ cd drivers
    $ git clone https://github.com/zxcmehran/FPGADisplay-driver.git display_handler_v1_00_a

After cloning, just add `display_handler.h` header file and include it in your application code.

**Note:** This driver needs the main IP Core to operate. Check [FPGADisplay-ipcore](https://github.com/zxcmehran/FPGADisplay-ipcore) repository for installation details.


## License
This is a B.Sc Project of Electrical Engineering by **Mehran Ahadi**. Its published under
**[MIT](https://tldrlegal.com/license/mit-license) license**. Please include the copyright and the license notice in all copies or substantial uses of the work.
