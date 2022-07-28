/**
 * TODO: file header
 *
 * Author: Enzo Han
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include <stdlib.h>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;
/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(const string& inFileName, const string& outFileName) {
    char* line;
    ifstream ifs;
    ofstream out;
    unsigned int parsed;
    ifs.open(inFileName, ifstream::in);
    vector<unsigned int> freq(256);
    for (int i = 0; i < freq.size(); i++) {
        ifs.getline(line, 256);
        parsed = atoi(line);
        freq[i] = parsed;
    }
    HCTree tree;
    tree.build(freq);
    vector<unsigned char> message;
    while (!ifs.eof()) {
        if (ifs.peek() == EOF) {
            break;
        }
        message.push_back(tree.decode(ifs));
    }
    ifs.close();
    out.open(outFileName);
    for (int i = 0; i < message.size(); i++) {
        out << message[i];
    }
    out.close();
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(const string& inFileName, const string& outFileName) {
    char* line;
    ifstream ifs;
    ofstream out;
    ifs.open(inFileName, ifstream::in);
    // BitInputStream is(ifs, 4000);

    // vector<unsigned int> freq(256);
    // my first line tells me how many lines i need to parse
    // ifs.getline(line, 256);
    // int parsed = atoi(line);
    // int symbol;
    // int frequency;
    /**
    for( int i = 0; i < parsed; i++) {
            ifs.getline(line, 256);
            symbol = (int)((unsigned int)*line);
            line++;
            frequency = (int)*line;
            freq[symbol] = frequency;
    }
    **/
    ifs.getline(line, 256);
    int unique = atoi(line);
    ifs.getline(line, 256);
    int parsed = atoi(line);
    HCTree tree;
    if (unique > 1) {
        tree.rebuild(ifs);
    } else {
        tree.uniquerebuild(ifs);
    }
    vector<unsigned char> message;
    // char c;
    // ifs.get(c);
    // cout << (int)((unsigned char)c) << endl;
    // ifs.get(c);
    // cout << (int)((unsigned char)c) << endl;
    BitInputStream is(ifs, 4000);
    /**
    while(!is.eof()) {
            message.push_back(tree.decode(is));
            if(is.eof()) {
                    break;
            }
    }
    **/
    for (int i = 0; i < parsed; i++) {
        message.push_back(tree.decode(is));
    }
    ifs.close();
    out.open(outFileName);
    // cout << message.size() << endl;
    for (int i = 0; i < message.size(); i++) {
        out << message[i];
    }
    out.close();
}

/* Main program that runs the decompression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Uncompresses files using Huffman Encoding");
    options.positional_help(
        "./path_to_compressed_input_file ./path_to_output_file");

    bool isAscii = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Read input in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAscii))("input", "",
                                       cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit.");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // if compressed file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        exit(0);
    }

    if (isAscii) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }

    return 0;
}
