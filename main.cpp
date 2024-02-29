// C++ program for Huffman Coding
#include <cstdlib>
#include <set>
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;

#define MAX_TREE_HT 100

// Một nút của cây Huffman
struct MinHeapNode {

	// Một ký tự đầu vào
	char data;

	// Tần số xuất hiện của ký tự
	unsigned freq;

	// Nút con trái, phải
	struct MinHeapNode *left, *right;
};

// A Min Heap: Tập hợp các nút min-heap (hoặc cây Huffman)
struct MinHeap {
	// Kích thước hiện tại của min heap
	unsigned size;
	// dung lượng của min heap
	unsigned capacity;
	// Cin trỏ vào mảng của Min heap
	struct MinHeapNode** array;
};

//  Hàm tiện ích cấp phát một nút min heap mới với ký tự và tần số của ký tự đã cho
struct MinHeapNode* newNode(char data, unsigned freq)
{
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(
		sizeof(struct MinHeapNode));
	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}

// Hàm tiện ích để tạo một min heap với dung lượng đã cho
struct MinHeap* createMinHeap(unsigned capacity)

{
	struct MinHeap* minHeap
		= (struct MinHeap*)malloc(sizeof(struct MinHeap));

	// current size is 0
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = (struct MinHeapNode**)malloc(
		minHeap->capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// Hàm hoán đổi hai nút Minheap
void swapMinHeapNode(struct MinHeapNode** a,
					struct MinHeapNode** b)

{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// Hàm Minheap
void minHeapify(struct MinHeap* minHeap, int idx)

{

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size
		&& minHeap->array[left]->freq
			< minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size
		&& minHeap->array[right]->freq
			< minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// Kiểm tra kích thước của Minheap có bằng 1 không
int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

// Lấy ra giá trị nhỏ nhất từ Heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{
	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	--minHeap->size;
	minHeapify(minHeap, 0);
	return temp;
}

// Chèn một nút mới vào Minheap
void insertMinHeap(struct MinHeap* minHeap,
				struct MinHeapNode* minHeapNode)
{
	++minHeap->size;
	int i = minHeap->size - 1;
	while (i
		&& minHeapNode->freq
				< minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	minHeap->array[i] = minHeapNode;
}

// Hàm xây dựng Minheap
void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;
	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// In mảng 
void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		cout << arr[i];
	cout << "\n";
}

// Hàm tiện ích để kiểm tra xem nút này có phải là nút lá hay không
int isLeaf(struct MinHeapNode* root)
{
	return !(root->left) && !(root->right);
}

// Tạo một min heap có dung lượng bằng với kích thước và chèn tất cả các ký tự của mảng data[] vào min heap.
// Kích thước lúc đầu của Minheap bằng với dung lượng đã cho
struct MinHeap* createAndBuildMinHeap(char data[],
									int freq[], int size)
{

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// Hàm xây dựng cây Huffman
struct MinHeapNode* buildHuffmanTree(char data[],
									int freq[], int size)
{
	struct MinHeapNode *left, *right, *top;
	// Step 1: Tạo một min heap có dung lượng bằng với kích thước. 
	// Ban đầu, có số lượng nút bằng với kích thước đã cho.
	struct MinHeap* minHeap
		= createAndBuildMinHeap(data, freq, size);
	// Lặp lại cho đến khi kích thước của heap trở thành 1.
	while (!isSizeOne(minHeap)) {
		// Step 2: Bỏ ra hai phần tử có tần số nhỏ nhất từ min heap.
		left = extractMin(minHeap);
		right = extractMin(minHeap);
		// Step 3: "Tạo một nút nội bộ mới với tần số bằng tổng của hai nút có tần số vừa được lấy ra. 
		// Làm cho hai nút đã lấy ra trở thành con trái và con phải của nút mới này. 
		//Thêm nút mới này vào min heap. 
		//'$' là một giá trị đặc biệt dành cho các nút nội bộ, không được sử dụng."
		top = newNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}
	// Step 4: Nút còn lại là nút gốc và cây đã hoàn thành.
	return extractMin(minHeap);
}

// In mã huffman từ gốc của cây Huffman.
// Sử dụng mảng arr để lưu mã Huffman.
void printCodes(struct MinHeapNode* root, int arr[],
				int top)

{

	// Gán giá trị 0 cho cạnh trái và tiếp tục đệ quy.
	if (root->left) {
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Gán giá trị 1 cho cạnh phải và tiếp tục đệ quy.
	if (root->right) {
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

    //Nếu đây là một nút lá, thì nó chứa một trong các ký tự đầu vào, in ra ký tự và mã của nó từ mảng arr[].
	if (isLeaf(root)) {
		cout << root->data << ": ";
		printArr(arr, top);
	}
}

// Hàm chính xây dựng một cây Huffman và in ra mã bằng cách duyệt cây Huffman đã xây dựng.
void HuffmanCodes(char data[], int freq[], int size)
{
	// Xây dựng cây Huffman
	struct MinHeapNode* root
		= buildHuffmanTree(data, freq, size);

	// In ra Mã Huffman với cây Huffman xây dựng ở trên
	int arr[MAX_TREE_HT], top = 0;

	printCodes(root, arr, top);
}

// Driver code
int main()
{
    string filename = "Text.txt";
    ifstream file(filename);

    if (!file) {
        cerr << "Không thể mở tệp: " << filename << endl;
        return 1;
    }

    // Đếm số lần xuất hiện của tất cả các ký tự trong tệp
    unordered_map<char, int> charCount;
    char ch;
    while (file >> noskipws >> ch) {
        if (isalnum(ch)) { // Nếu muốn đếm tất cả các ký tự, bạn có thể bỏ điều kiện này
            charCount[ch]++;
        }
    }
    file.close();

    // Tính độ dài của mảng (số lượng ký tự khác nhau trong văn bản)
    int arrayLength = charCount.size();

    // Khởi tạo hai mảng để lưu ký tự và số lần xuất hiện
    char* arr = new char[arrayLength];
    int* freq = new int[arrayLength];

    // Điền thông tin vào hai mảng
    int index = 0;
    for (const auto& entry : charCount) {
        arr[index] = entry.first;
        freq[index] = entry.second;
        index++;
    }

    // Hiển thị ký tự và số lần xuất hiện tương ứng
    cout << "Độ dài mảng: " << arrayLength << endl;
    cout << "Ký tự và số lần xuất hiện:" << endl;
    for (int i = 0; i < arrayLength; ++i) {
        cout << arr[i] << ": " << freq[i] << endl;
    }

	cout<<'\n'<< arrayLength << " and " << sizeof(arr[0]) << '\n' <<endl;
    
    int size = arrayLength / sizeof(arr[0]);

	HuffmanCodes(arr, freq, size);
	

	return 0;
}

