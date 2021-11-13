# CP6-SuperProject

See the [newspaper development](doc/README.md).

See the [diagram](https://drive.google.com/file/d/1NRC8RcxTDao6eo0wLMJNhgn8msu9BmPU/view?usp=sharing).

### Authors

See the [authors](doc/AUTHORS.md) of the project.

### Requirements
Make sure that you have installed the following libs for this project:
- cmocka for unit testing (docs [here](https://cmocka.org)):
```sh 
$ wget https://cmocka.org/files/1.1/cmocka-1.1.3.tar.xz
$ tar -xJf cmocka-1.1.3.tar.xz
$ cmake cmocka-1.1.3
$ sudo make install 
```
- ncurses for texting interface:
```sh
$ sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Launch
- Launch the main program
```sh
$ make 
$ make run arg="prog1 prog2 prog3 prog4"
```
- Launch the unit tests
```sh
$ make compile-tests
$ make run-tests 
```
See the ```Makefile``` for more information.