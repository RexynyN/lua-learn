cmake_minimum_required(VERSION 3.25.0)

project(cottonml)
add_executable(cottonml linearalg.c stats.c counter.c utils.c knn.c dataset.c)

# Links the libraries to the .exe
target_link_libraries(cottonml m icuuc icuio) 
