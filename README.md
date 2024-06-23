可以使用
$$gprof -b ./cmake-build-debug/${name}./cmake-build-debug/gmon.out|gprof2dot -s|dot -Tpng -o
./debug_png/${name}.png$$ 来生成图