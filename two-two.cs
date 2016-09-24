using System;
using System.Collections.Generic;
using System.IO;
using System.Numerics;

public class Trie {
    public Trie[] nodes;
    public int isInDictionary;
    public int isContinued;

    public Trie() {
        isInDictionary = 0;
        isContinued = 0;
    }

    public void AddString(string str, int index = 0) {
        if (isContinued == 0) {
            isContinued = 1;
            nodes = new Trie[10] { new Trie(), new Trie(), new Trie(), new Trie(), new Trie(),
                                   new Trie(), new Trie(), new Trie(), new Trie(), new Trie() };
        }
        
        int digit = str[index] - '0';
        if (str.Length - index == 1) {
            nodes[digit].isInDictionary = 1;
        } else {
            nodes[digit].AddString(str, index + 1);
        }
    }
    
    public int CountStringsAt(string str, int index) {
        if (isContinued == 0  || index >= str.Length)
            return 0;
        int digit = str[index] - '0';
        return nodes[digit].isInDictionary + nodes[digit].CountStringsAt(str, index + 1);
    }
}

class Solution {
    static void Main(String[] args)      {
        Trie tries = new Trie();
        BigInteger x = new BigInteger(1);
        for (int i = 0; i <= 800; ++i) {
            tries.AddString(x.ToString());
            x = BigInteger.Multiply(x, 2);
        }
        
        int testCases = Convert.ToInt32(Console.ReadLine());
        for (int t = 0; t < testCases; ++t) {
            string str = Console.ReadLine().Trim();
            int length = str.Length;
            int count = 0;
            for (int i = 0; i < length; ++i) {
                count += tries.CountStringsAt(str, i);
            }
            Console.WriteLine(count);
        }
    }
}
