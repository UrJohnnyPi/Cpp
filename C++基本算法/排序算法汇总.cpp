/*
冒泡排序
*/
void bubble_sort(int * arr, int len) {
	for(int i = 0; i < len - 1; ++i) {
		for(int j = 0; j < len - 1 - i; ++j)
			if(arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
	}
}

/*
选择排序
*/
void select_sort(int * arr, int len) {
	for(int i = 0, min; i < len - 1; ++i) {
		min = i;
		for(int j = i + 1; j < len; ++j)
			if(arr[j] < arr[min])
				min = j;
		if(min != i)
			swap(arr[i], arr[min]);
	}
}

/*
插入排序
*/
void insertion_sort(int * arr, int len) {
	for(int i = 1; i < len; ++i) {
		int j;
		for(j = i - 1; j >= 0 && arr[j] > arr[i]; --j);
		swap(arr[j + 1], arr[i]);
	}
}

/*
希尔排序
*/
void shell_sort(int * arr, int len) {
	for(int gap = len / 2; gap > 0; gap /= 2) {
		for(int i = gap; i < len; ++i) {
			for(int j = i - gap; j >= 0; j -= gap) {
				if(arr[j] > arr[j + gap])
					swap(arr[j], arr[j + gap]);
			}
		}
	}
}

/*
归并排序
*/
void merge(int * arr, int low, int mid, int high) {
	vector<int> letf(arr + low, arr + mid + 1);
	vector<int> right(arr + mid + 1, arr + high + 1);
	auto liter = left.begin(), riter = right.begin();
	left.insert(left.end(), numeric_limits<int>::max());
	right.insert(right.end(), numeric_limits<int>::max());
	for(int i = low; i <= high; ++i) {
		if(* liter < * riter) {
			arr[i] = * liter;
			++liter;
		} else {
			arr[i] = * riter;
			++riter;
		}
	}
}
void merge_sort(int * arr, int low, int high) {
	if(low < high) {
		int mid = (low + high) / 2;
		merge_sort(arr, low, mid);
		merge_sort(arr, mid + 1, high);
		merge(arr, low, mid, high);
	}
}

/*
快速排序
*/
int partition(int * arr, int low, int high) {
	int pivot = arr[low];
	while(low < high) {
		while(low < high && arr[high] >= pivot) ++high;
		arr[low] = arr[high];
		while(low < high && arr[low] <= pivot) ++low;
		arr[high] = arr[low];
	}
	arr[low] = pivot;
	return low;
}
void quick_sort(int * arr, int low, int high) {
	if (low < high) {
		int pivotpos = partition(arr, low, high);
		quick_sort(arr, low, pivotpos - 1);
		quick_sort(arr, pivotpos + 1, high);
	}
}

/*
堆排序
*/
