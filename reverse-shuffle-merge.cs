using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
class Solution {
    static void Main(String[] args) {
        var s = Console.ReadLine();
        Dictionary<char, int> d = new Dictionary<char, int>(), allowance;
        for (int i = 0; i < s.Length; i++){
            if (!d.ContainsKey(s[i])){
                d.Add(s[i], 1);
            }
            else{
                d[s[i]]++;
            }
        }
        d = d.ToDictionary(pair => pair.Key, pair => pair.Value / 2);
        allowance = new Dictionary<char, int>(d);
        var result = string.Empty;
        var smallest = d.Where(x => x.Value != 0).Min(x => x.Key);
        bool toadd = false;
        for (int i = s.Length - 1; i >= 0; i --){
            if (d[s[i]] > 0){
                if (s[i] == smallest || allowance[s[i]] == 0){
                    toadd = true;
                }
                else if (i - 1 >= 0){
                    var temp = new Dictionary<char, int>(allowance);
                    for (int j = i - 1; j >= 0; j--){
                        if (d[s[j]] == 0){
                            continue;
                        }
                        if (s[j] < s[i]){
                            break;
                        }
                        if (temp[s[j]] == 0){
                            toadd = true;
                            break;
                        }
                        temp[s[j]]--;
                    }
                }
            }
            if (toadd){
                d[s[i]]--;
                result += s[i];
                if (d[smallest] == 0){
                    smallest = d.Any(x => x.Value != 0) ? d.Where(x => x.Value != 0).Min(x => x.Key) : d.Max(x => x.Key);
                }
            }
            else{
                allowance[s[i]]--;
            }
            toadd = false;
        }
        Console.WriteLine(result);
    }
}
