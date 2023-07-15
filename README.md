In this assignment, I have implemented a plagiarism checker using a well know similarity metric system i.e. cosine similarity.
I have also used tf-idf vectorisation to put weights on the words that are unique and to disregard common words like the, a, then etc.

First, I parse all the corpus files in the folder and the test file and get a vector with each of their term frequencies. Now I use all these vectors to get inverse document frequencies and then get tfidf vector for each of these files. Now for all the files in the corpus I calculate cosine similarity with the test file and print them.
The files with more than 15% similarity are considered plagiarised.
