KIZITO MARK JAMES 25/U/30991/PS
Dataloading.
This is the process of transfering data from the CPU ram to the GPU.
Basically getting data from storage turning it to data tenors and sendig it to the GPU in mini batches.

Process
storage-> dataset-> dataloader-> RAM(pinned memory)-> GPU

Pipline movement
-Storage access:Reading of rawbytes from network storage or disk.
-Preprocessing and Argumentation:Decoding bytes, cropping images, tokenizing text or applying argumentation on the CPU.
-Batching and structuring:Grouping individual items into multidimensional batch tenors.
-Host to Device transfer:Moving Batch tenors from CPU memory to GPU memory.
In Dataloading, its mainly split in two.Datasets and Dataloaders.

Datasets
Define what the data is about and how to process it.
-Map style Datasets(__len__ and __getline__):This implements index lookup, given an index i. it reads i from the disk/ storage and starts transformation
Ideal when data size is known.
-iterable Datasets(__iter__):This implements astreaming interface, where data is read from the disk/ storage and transformed on the fly.
Ideal for massive datasets that cannot be indexed

Dataloader
-Batching 
-Shuffling
-Multiprocessing
-Memory pinning
-Collation

CVS Dataloading(Converting CVS Datasets to c++)
Turning CVS datasets into C++ uses libraries like std::ifstream for file input
std::stringstream with std::getline for line and column tokenization

JSON Dataloading(Converting JSON Datasets to c++)
Turning JSON datasets into C++ only needs header library nlohmann/json. (#include <nlohmann/json.hpp>)

Member Dependency
Since my Dataloading part is the first, i don't have any dependency on any other member.
As for others depending on my work;
-Member 2
Depends on the Dataframes and the column sturctures to extact data vectors, compute means, quantities.
-Members 3 and 4
Depend on Dataframe column indexing to feed raw numerical data into there plotting functions.
-Member 7
Depends on CSV reader to write full piple example and run fu;l intergration tests.

Roadmap
Read and research about dataloading in C++.
Architecture and core file I/O operations.Implementing Map style Datasets and Iterable Datasets.
Paraphrasing, typecasting and data transformation and Data validation.
Memory optimisation and Batching techniques. Implementing Dataloader with multiprocessing and memory pinning.
COncurency and Asynchronous data loading techniques(Asynchronous pipline). Implementing Collation and Shuffling techniques.
Error handling and logging.
Optimisation and data review.

AI use and tools
Gemini:To research efficient C++ data strucutres, CSV parasing.
