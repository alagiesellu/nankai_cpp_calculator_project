# Project forked from [TheLartians/PEGParser](https://github.com/TheLartians/PEGParser).

# To Execute Project
Execute the following bash command;
```bash
cmake -Scalculator -Bbuild/calculator && cmake --build build/calculator -j8 && ./build/calculator/main
```

# To Execute Project in Docker
Just run the dockerfile;
```bash
docker run -it $(docker build -q .)
```

