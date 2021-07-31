#include"missingData.h"

const std::vector<float>& handleMissing(const std::vector<float>& v, std::vector<float>& v2){
    int l = v.size();
    for (int i = 0; i < l; i++)
    {
        if (v[i] != -1.0)
        {
            v2.push_back(v[i]);
        }
    }
}
