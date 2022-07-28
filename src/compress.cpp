/**
 * TODO: file header
 *
 * Author: Enzo Han
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    vector<unsigned int> freq(256);
    ifstream ifs;
    ifs.open(inFileName, ifstream::in);  // might not need ifstream::in??
    char c;
    while (ifs.get(c)) {
        freq[(int)((unsigned char)c)]++;
        // cout << "first " << ((unsigned char)c) << endl;
        // cout << "second " << (int)((unsigned char) c) << endl;
    }
    ifs.close();
    //
    vector<unsigned int> original;
    char d;
    ifs.open(inFileName, ifstream::in);
    while (ifs.get(d)) {
        original.push_back((int)((unsigned char)d));
    }
    //
    ofstream outfile;
    outfile.open(outFileName);
    for (int i = 0; i < freq.size(); i++) {
        outfile << freq[i];
        outfile << "\n";
    }
    HCTree tree;
    tree.build(freq);
    for (int i = 0; i < original.size(); i++) {
        tree.encode(original[i], outfile);
    }
    ifs.close();
    outfile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {
    vector<unsigned int> freq(256);
    vector<unsigned int> original;
    ifstream ifs;
    ifs.open(inFileName, ifstream::in);
    // BitOutputStream os(inFileName, 4000);
    int characters = 0;
    int unique = 0;
    char c;
    while (ifs.get(c)) {
        freq[(int)((unsigned char)c)]++;
        original.push_back((int)(unsigned char)c);
    }
    for (int i = 0; i < freq.size(); i++) {
        characters += freq[i];
        if (freq[i] > 0) {
            unique++;
        }
    }
    // cout << characters << endl;
    ofstream outfile;
    outfile.open(outFileName);
    BitOutputStream os(outfile, 4000);
    /**
    int lineNum = 0;
    for(int i = 0; i < freq.size(); i++) {
            if(freq[i] > 0) {
                    lineNum++;
                    cout << "i is " << i << endl;
                    cout << freq[i] << endl;
            }
    }
    **/
    // cout << "lineNum is: " << lineNum << endl;
    /**
    outfile << lineNum;
    outfile << "\n";
    for(int i = 0; i < freq.size(); i++) {
            if(freq[i] > 0) {
                    outfile << (char)i;
                    outfile << (char)freq[i];
                    outfile << "\n";
            }
    }
    **/
    HCTree tree;
    tree.build(freq);
    string str = tree.buildString();
    outfile << unique;
    outfile << "\n";
    outfile << characters;
    outfile << "\n";
    outfile << str;
    // STORE THE TREE IN THE HEADER
    // 1 REPRESENTS LEAF, 0 REPRESENTS NON-LEAF
    for (int i = 0; i < original.size(); i++) {
        tree.encode(original[i], os);
    }
    os.flush();
    ifs.close();
    outfile.close();
}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}
