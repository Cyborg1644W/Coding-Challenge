#include <iostream> 

using std::cout; 
using std::cin; 
using std::endl; 
using std::string; 

void Products(int bread1, string *bread2); 
int Sales(int sales1, int *sales); 
void Summary(int bread, int *sales, string *bread3);

int main(){

    const int bread = 5; 
    int tinapaySales[bread] = {}; 
    int *sales = tinapaySales; 

    string tinapay[bread] = {"Monay", "Pandesal", "Ensaymada", "Empanada", "Bonete"}; 
    string *woah = tinapay; 

    Products(bread, woah); 
    int HowMany = Sales(bread, sales);
    Summary(HowMany, sales, woah);
}

void Products(int bread, string *woah){ 

    cout << "------------- Floof Bakery -------------" << endl; 
    cout << "----------- List of products -----------" << endl; 

        for(int i = 0; i < bread; i++){
             cout << i + 1 << ". " << *woah << endl; 
             woah++; 
       }
    
    cout << "---------------------------------------" << endl; 
}

int Sales(int bread, int *sales){

    int NumSales;
    int ProNum; 

    cout << "How many product will you record?" << endl; 
    cout << "Enter the number: ";  
    cin >> NumSales; 

        for (int i = 0; i < NumSales; i++){

            cout << "---------------------------------------" << endl;
            cout << "Product " <<  i + 1 << endl;
            cout << "Please enter the product number: ";
            cin >> ProNum; 
            cout << "Please enter the product sales: "; 
           
            for(int i = 0; i < ProNum; i++){
            (*sales)++; 
            }
            cin >> *sales; 
            
            for(int i = 0; i < ProNum; i++){
            (*sales)--; 
            }
        }
    
    return NumSales; 
}

void Summary(int HowMany, int *sales, string *woah){

    cout << "---------------------------------------" << endl;
    for (int i = 0; i < HowMany; i++){
        cout << *woah << ":" << *sales << endl; 
         
        woah++;
        }
}
