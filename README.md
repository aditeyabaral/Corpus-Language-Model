# Corpus-Language-Model
A C project that creates a language model out of any given corpus of text by converting it into Markov chains.

This is a more general implementation of my [WhatsApp Chat Language Model](https://github.com/aditeyabaral/WhatsApp-Chat-Language-Model "WhatsApp Chat Language Model") repository and can similarly be used to autocomplete sentences.

# Markov Chains and Language Modelling
A Markov chain is a stochastic model describing a sequence of possible events in which the probability of each event depends only on the state attained in the previous event. 

This translates to
_P( i | i-1 , i-2 , i-3, ..., 0) = P( i | i-1)_

A language model can be built on this principle by using the concept of n-grams and Markov Chains where the last _i-n_ states are taken into consideration to build the model. Thus, for each n-gram occuring in the corpus of text, a state is created and a set of all possible _(n+1)th_ words is obtained along with their probabilities, which is used to map these states with their respective highest probabilitic word. Thus by starting at any state, a sequence of text can be recursively generated.

The value of _n_ can be modified, however trigrams and bigrams work better since as this value of _n_ increases, more context is included into the states and stricter matching is performed, leading to overfitting of the corpus. 

# Compilation
* Place any text file in the same directory as the source code which you would like to model.
* Compile and execute with the following commands:
```
gcc MarkovModel.c
./a.out
```
