/*
    Includes are not necessary for the operation of DataRow.
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct DataRow {
    private:
    u_int16_t data = 0;
    
    void initData(bool values[], int n){
        for(int i = 0; i < n; i++) data |= (values[i] << i);
    }

    public:
    DataRow(bool values[], int n) {
        initData(values, n);
    }

    DataRow(bool i0, bool i1, bool i2, bool i3, bool i4, bool i5, bool i6, bool i7, bool i8, bool i9){
        bool values[] = {i0, i1, i2, i3, i4, i5, i6, i7, i8, i9};
        initData(values, 10);
    }
    DataRow() = default;

    bool getEntry(int index) {
        return data >> index & 1;
    }
};

int main() {
    DataRow row = {0,1,0,0,1,1,0,0,0,1};

    for(int i = 0; i < 10; i++){
        cout << row.getEntry(i);
    }
    cout << endl;
    cout << "Size of DataRow: " << sizeof(row) << " bytes\n";

    return 0;
}