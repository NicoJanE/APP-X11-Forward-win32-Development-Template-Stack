**Item: 2** ***Passing instance data to static WndProc (Callbacks)*** <br> 
Callbacks of the WndProc should be defined static, see [item 1](win32-callbacks.md) so we need away to pass the instance data.
We use Dependency Injection or the Visitor pattern, where the dependency inhjection is the fist choice

<small>*Sample dependency Injection*
```
// classs with static method and depenency
class GUI-WinMain
{
    int nDefaultSaldo;
    GUI-WinMain(int value) : nDefaultSaldo(value) {}
    
    static void DisplaySaldo(GUI-WinMain& obj)
    { obj.nDefaultSaldo =+ 100;  }
}

// Calling
int main()
{
    GUI-WinMain obj1(-100);
    GUI-WinMain obj2(0);

    GUI-WinMain::DisplaySaldo(obj1);
    GUI-WinMain::DisplaySaldo(obj2);

}
```
</small>
<br>

<small>*Sample Visitor pattern*
```
#include <iostream>
class GUI_WinMain;

class Visitor
{
    public:
    void visit(GUI_WinMain& obj);  // Declare visit method
};


// Define visit method for modifying instance data
void Visitor::visit(GUI_WinMain& obj)
    { obj.nDefaultSaldo *= 2;  }

// Class with a static method and visitor pattern applied
class GUI_WinMain
{
    private:
    int nDefaultSaldo;

    public:
    GUI_WinMain(int value) : nDefaultSaldo(value) {} // CTOR

    static void DisplaySaldo(GUI_WinMain& objInstance, Visitor& objVisitor)
        { objVisitor.visit(objInstance);  // Use the visitor to modify instance data }

    void PrintSaldo() const
        { std::cout << "Saldo: " << nDefaultSaldo << std::endl; }
};

// Main function.
int main()
{
    GUI_WinMain obj1(100);      // Create an object with initial saldo
    Visitor visitor;            // Create a visitor object

    obj1.PrintSaldo();          // Print the original saldo

    // Use the static method to apply the visitor to the instance
    GUI_WinMain::DisplaySaldo(obj1, visitor);
    obj1.PrintSaldo();          // Print the modified saldo

    return 0;
}

```
</small>



*External reference:*
 - [How to Windproc as class method](https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function)
 - [Mapping Wndproc to your class](https://www.codeproject.com/Articles/17894/Simple-Mapping-of-WndProc-to-your-Specific-Class-W)
