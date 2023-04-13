# R tree search engine

A R tree search engine written by C++.

## Basic Information

There are many libraries which can solve spatial searching querys. But their performance may be bounded when facing larger scale of dataset and queries.

R tree search engine is a C++ library that aims to provide an efficient and scalable spatial search engine which can serve as a building block of map application.

## Problem to Solve

When we talked about map application, often time we'll have queries like “return all buildings in this area”, “find 1000 closest gas stations to this point”, and returning results within milliseconds even when searching millions of objects.

The goal of R tree search engine is to provide a high performance (The goal is to make the query time as low as possible) search engine to solve these classical problems like "K nearest neighbors" and "range query".

## Prospective Users

This project aim to provide developers that want to build their own map service a cornerstone.

## System Architecture

In python, mostly of the outputs and inputs are given through dictionary.

In C++, the library will provide class as interface of the library, especially output.


## API Description

This project will provide support at both C++ and Python. 

For C++ user, this library can be include by linking the library, for Python user, this library can be used by import and pass in the query parameter by using dictionary.

## Engineering Infrastructure

The project uses CMake for automatic build system, enabling users to easily
build the library on their respective platforms. The library is hosted on
GitHub and uses Git for version control, enabling users to easily collaborate
on the codebase. Besides that, the project use pytest for unit testing. Continuous integration is utilized for testing the library
across multiple platforms.

Github Action is configured for running the tests regularly and for
each check-in and pull request.

All new features developed will have corresponding unit tests.

## Schedule

### Week 1 (4/10)

- Read the reference readings to have better understanding about the data structure.
- Search existing R tree module, tried to run benchmark for them.
- Check how those libraries provide their interface.

### Week 2 (4/17)

- Won't be doing anything due to I need to participate a hackathon called ETHTaipei

### Week 3 (4/24)

- Produce test dataset for essential features.
- Plan out the prototype interface.
- Write c++ classes that state out the member functions and variables.
 
### Week 4 (5/1)

- Developing the essential features for the library.

### Week 5 (5/8)

- Optimize the performance.

### Week 6 (5/15)

- Refactor.

### Week 7 (5/22)

- Complete API documentation for the library, and API document also serve as the slide for presentation.

### Week 8 (5/29)

- Complete the slide for upcoming presentation

- Collect performance analysis data and plot the result. 

- Plan out future roadmap.

## References

* [Vladmir Agafonkin (2017). A dive into spatial search algorithms](https://blog.mapbox.com/a-dive-into-spatial-search-algorithms-ebd0c5e39d2a)
* [arwa A. M. Abd Elwahab, Khaled M. Mahar, Hatem Abdelkader (2013), Hatem Awad Khater. Combining R-Tree and B-Tree to Enhance Spatial Queries Processing](https://ieeexplore.ieee.org/document/9529800)

