#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include <conio.h>

using namespace std;

//Struct to store the x and y coordiantes of a Point
struct Point {
	float x;
	float y;
};

//Struct to store the distance between two groups and the index of groups
struct PointAct {
	float distance=0;
	int g1;
	int g2;
};

//Heap Class
class Heap {
	PointAct * h;   //Array to be used by heap functions contains distances and groups 
  	int hsize;         //elements in array   
public:
	//Default Constructor
	Heap() {
		h = nullptr;
		hsize = 0;
	} 
   //Parameterized Constructor
	Heap(int size) {
		h = new PointAct[size];
		hsize = 0;
	}

	//Function which balances the Heap by using index, and it is also updating indexes in matrix
	void Heapify(int i,int ** & matrix) {
		int min = i;
		if (2 * i <= hsize && h[2 * i].distance < h[min].distance)
			min = 2 * i;
		if (2 * i +1 <= hsize && h[2 * i+1].distance < h[min].distance)
			min = 2 * i+1;
		if (min != i) {
			int a = h[i].g1;
			int b = h[i].g2;
			int c = h[min].g1;
			int d = h[min].g2;
			if (a > b) {
				int e = a;
				a = b;
				b = e;
			}
			if (c > d) {
				int e = c;
				c = d;
				d = e;
			}
			swap(matrix[a][b], matrix[c][d]);
			swap(h[i], h[min]);
			Heapify(min,matrix);
		}
	}
	//Function to simply insert in heap
	void insert(PointAct P) {
		hsize++;
		int i = hsize;
		h[hsize] = P;
		while (i > 1 && h[i].distance < h[i / 2].distance) {
			swap(h[i], h[i / 2]);
			i = i / 2;
		}
	}

	//Function to insert in heap and it is also updating indexes in matrix
	void insert(PointAct P,int **matrix) {
		hsize++;
		int i = hsize;
		h[hsize] = P;
		int x = h[hsize].g1;
		int y = h[hsize].g2;
		if (x > y) {
			int e = x;
			x = y;
			y = e;
		}
		matrix[x][y] = hsize;
		while (i > 1 && h[i].distance < h[i / 2].distance) {
			int a = h[i].g1;
			int b = h[i].g2;
			int c = h[i/2].g1;
			int d = h[i/2].g2;
			if (a > b) {
				int e = a;
				a = b;
				b = e;
			}
			if (c > d) {
				int e = c;
				c = d;
				d = e;
			}
			swap(matrix[a][b], matrix[c][d]);
			swap(h[i], h[i / 2]);
			i = i / 2;
		}
	}
	//return current size of heap
	int getsize() {
		return hsize;
	}
	//to resize the Heap
	void resize(int size) {
		h = new PointAct[size];
		hsize = 0;
	}
	//it returns the minmum distance and the corresponding groups
	PointAct getmin() {
		return h[1];
	}

   //Function to make min heap of distances between the groups.... includes distances and corresponding groups
	void buildarray(int n,int ** &matrix,Point * &arr, vector <list<int>> & L) {
		matrix = new int *[n + 1];
		for (int i = 1; i <= n; i++) {
			matrix[i] = new int [n+1];
		}
		L.resize(n + 1);
		for (int i = 1; i <= n; i++) {
			L[i].push_front(i);
		}
		for (int i = 1; i < n+1; i++) {
			for (int j = i+1; j <= n; j++) {
				float a = sqrtf(((arr[i].x - arr[j].x) * (arr[i].x - arr[j].x)) + ((arr[i].y - arr[j].y) * (arr[i].y - arr[j].y)));
				PointAct P;
				P.distance = a;
				P.g1 = i;
				P.g2 = j;
				insert(P,matrix);
			}
		}
	}
	//Function to remove certain element from the heap and it is also updating indexes in matrix
	void remove(int index , int **& matrix) {
		int a = h[index].g1;
		int b = h[index].g2;
		int c = h[hsize].g1;
		int d = h[hsize].g2;
		swap(h[index], h[hsize]);
		if (a > b) {
			int e = a;
			a = b;
			b = e;
		}
		if (c > d) {
			int e = c;
			c = d;
			d = e;
		}
		swap(matrix[a][b], matrix[c][d]);
		hsize--;
		Heapify(index,matrix);
	}

	//Destructor
	~Heap() {
		delete[] h;
	}

	friend class Star;
};

//Star class
class Star {
	Point * arr;   //Array to store all points x and y coordiantes 
	vector <list<int>> L;  //vector of linked list
	int **matrix;     //matrix to maintain heap indexes 
	Heap H;
	int n;
	int m;
public:
	//Constructor
	Star() {
		arr = nullptr;
		matrix = nullptr;
		n = 0;
		m = 0;
	}

	//Function to read data from the file
	void readfile(string filename) {
		ifstream fin(filename);
		if (fin.fail()) {
			cout << "File not open" << endl;
		}
		else {
			fin >> n;
			//cout << n;
			char line[1000];
			fin.getline(line, 100);
			int a = 0;
			char temp0[50];
			int p = 0;
			while (line[a] != '\0') {
				if (line[a] == '#') {
					a++;
				}
				temp0[p] = line[a];
				a++;
				p++;
			}
			m = atoi(temp0);
			arr = new Point[n + 1];
			//cout <<groups;
			int index = 1;
		//	cout << "Points: " << endl;
			while (fin.getline(line, 1000)) {
				int i = 0;
				char temp[100];
				int k = 0;

				while (line[i] != '\0') {
					if (line[i] == '\t') {
						temp[k] = '\0';
						if (temp[0] != '\0') {
							if (atof(temp) != 0)
								arr[index].x = (atof(temp));
						}
						k = 0;
					}
					else {
						temp[k++] = line[i];
						if (line[i + 1] == '\0') {
							temp[k] = '\0';
							if (temp[0] != '\0') {
								if (atof(temp) != 0)
									arr[index].y = (atof(temp));
							}
						}
					}
					i++;
				}
                //cout << index << ": " << arr[index].x << "\t" << arr[index].y << endl;
				index++;
			}
		}
		int size = (n*(n - 1) / 2) + 1;
	    H.resize(size);
		H.buildarray(n,matrix,arr, L);
	}
    //it returns the array of groups
	Point * points() {
		return arr;
	}

	//Function to print the 2D Matrix
	void printMatrix() {
		for (int i = 1; i <= n; i++) {
			if (i == 0) {
				cout << setw(4);
			}
			for (int j = 1; j <= n; j++) {
				cout << matrix[i][j] << setw(4);
			}
			cout << endl;
		}
	}

	//Function to merge groups
	//it also updates distances in the heap
	void Calculations() {
		int a = 0;   int b = 0;
		for (int r = 0; r < n - m && H.getsize() > 0; r++) {
			PointAct real=H.getmin();
			a = real.g1;
			b = real.g2;
			for (int i = 1; i <= n; i++) {
				int i1 = 0;
				int i2 = 0;
				if (L[i].back() != -1) {
					if (i != a && i != b) {
						if (i <= a)
							i1 = matrix[i][a];
						if (i <= b)
							i2 = matrix[i][b];
						if (i > a)
							i1 = matrix[a][i];
						if (i > b)
							i2 = matrix[b][i];

						if (H.h[i1].distance > H.h[i2].distance) {
							H.h[i1].distance = H.h[i2].distance;
						}
						H.remove(i2,matrix);
					}
				}
			}
			H.remove(1,matrix);
			L[a].merge(L[b]);
			L[b].clear();
			L[b].push_back(-1);
			/*cout << "New:" << endl;
			for (int i = 1; i <= H.getsize(); i++) {
				cout << H.h[i].distance << " " << H.h[i].g1 << H.h[i].g2 << endl;
			}*/
		}
	}

	/*void u_mat(PointAct * arr) {
		for (int i = 1; i <= H.getsize() ; i++) {
			int a = arr[i].g1;
			int b = arr[i].g2;
			if (a<b)
				matrix[a][b] = i;
			if (a>b)
				matrix[b][a] = i;
		}
	}*/

	//Printing the merged groups
	void Print() {
		int u = 1; int col;
		HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);  //colorful printing
		SetConsoleTextAttribute(color, 4);              //setting color
		cout << endl << "Groups: " << endl;
		list <int> ::iterator it;
		for (int i = 1; i <= n; i++) {
			if (L[i].begin() != L[i].end() && L[i].front() != -1) {
				col = u + 4;
				if (col >= 7)
					col++;
				SetConsoleTextAttribute(color, col);
				cout << "Group " << u << ": ";
				u++;
			}
			for (it = L[i].begin(); it != L[i].end(); ++it) {
				int index = *it;
				if (index != -1) {
					cout << index << "(" << arr[index].x << "," << arr[index].y << ") ";
				}
			}
			if (L[i].begin() != L[i].end() && L[i].front() != -1) {
				cout << endl;
			}
			SetConsoleTextAttribute(color, 7);
		}
	}
	//it returns total number of groups
	int getsize() {
		return n;
	}
	//it returns the vector of the merged groups
	vector <list<int>> getVec() {
		return L;
	}
	//Destructor
	~Star() {
		delete[] arr;
		for (int i = 1; i <= n; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		L.~vector();
	}

};


//NOW ALL FUNCTIONS TO DISPLAY THE STARS IN VISUAL GRAPHICS 
//printing at certain x and y on console
void gotoxy(int x, int y)
{
	COORD coordinates; 
	coordinates.X = x;   
	coordinates.Y = y;     
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

//printing at certain x and y on console
void gotoxy(int xpos, int ypos, int col, char ch)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
	HANDLE  screen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(screen, col);
	DWORD useless;
	WriteConsole(screen, &ch, 1, (LPDWORD)&useless, NULL);
	SetConsoleTextAttribute(screen, 15);
	SMALL_RECT windowSize = { 0, 0, 100, 100 };
	SetConsoleWindowInfo(screen, TRUE, &windowSize);
}

//Used to print stars graphically
void Printstars(Point * arr, int size, vector <list<int>> L) {
	int col = 15;
	for (int i = 1; i <= size; i++) {
		float xpos = arr[i].x;
		float ypos = arr[i].y;
		gotoxy(30 + xpos ,18 - ypos, 15, '*');
	}
	 	_getch();
		list <int> ::iterator it;
		float xpos;
		int s=0;
		float ypos;
		int col2 = 5;
		for (int i = 1; i <= size; i++) {
			for (it = L[i].begin(); it != L[i].end(); ++it) {
				int index = *it;
				if (index != -1) {
					xpos = arr[index].x;
					ypos = arr[index].y;
					gotoxy(30 + xpos, 18 - ypos, col2, '*');
				}
		}
			if (L[i].begin() != L[i].end() && L[i].front() != -1) {
				col2++;
				if (col2 == 7)
					col2++;
			}
	  }
}
//used to print the boundary in which stars are to be printed
void printlayer(int xpos, int xpos2, int ypos, int ypos2, int col, int ch) {
	for (int i = xpos; i <= xpos2; i++) {
		gotoxy(i, ypos, col, ch);
		gotoxy(i, ypos2, col, ch);
	}
	for (int i = ypos; i <= ypos2; i++) {
		gotoxy(xpos, i, col, ch);
		gotoxy(xpos2, i, col, ch);
	}
	cout << endl;
}

//used to display stars
void Display() {
	ifstream fin("F:\\display.txt");
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, 9);
	if (fin.is_open())
		cout << fin.rdbuf();
	gotoxy(0, 15);
	SetConsoleTextAttribute(color, 7);
	printlayer(0, 79, 5, 17, 12, '|');
}

int main() {
	
	string filename = "F:\\input2.txt";
	Star a;
	Display();
	a.readfile(filename);
	a.Calculations();
	Printstars(a.points(), a.getsize(), a.getVec());
	gotoxy(40, 20, 0, 77);
	a.Print();
	
	return 0;
}