#include <iostream>
#include <fstream>
#include <string>
#include <vector>
std::string INTERS_TEXT[] = {
"M_0",
"M_1",
"M_2",
"U_0",
"U_1",
"U_2",
"C_0",
"C_1",
"C_2",
"T_0",
"T_1",
"T_2",
"T_12",
"T_02",
"T_01",
"B",
"S",
"L"
};

bool contains(std::vector<bool> v1, std::vector<bool> v2) {
	bool flag = true;
	for (size_t i = 0; i < v1.size(); i++) {
		if (v1[i] == true && v2[i] == false) {
			flag = false;
			break;
		}
	}
	return flag;
}

void int_to_vector(uint32_t t, std::vector<bool>& v) {
	for (size_t j = 18; j > 0; j--) {
		if (t & 1) {
			v[j - 1] = true;
		} else {
			v[j - 1] = false;
		}
		t = t >> 1;
	}
}

bool single_bit(uint32_t x) {
	for (uint32_t i = 0, j = 0; i < 18; i++) {
		if (x & 1) {
			j++;
		}
		if (j > 1) {
			return false;
		}
		x = x >> 1;
	}
	return true;
}

int main() {
	std::vector<std::vector<bool>> arr; // таблица аксиоматических функций
	std::ifstream input("table.txt"); // открываем файл с таблицей аксиоматических функций
	std::vector<bool> tmp;
	std::string line;
	if (input.is_open()) {
		while (getline(input, line)) {
			for(size_t i = 0; i < line.size(); i++) {
				if (line[i] == '1') {
					tmp.push_back(true);
				} else if (line[i] == '0') {
					tmp.push_back(false);
				}
			}
			arr.push_back(tmp);
			tmp.clear();
		}
	} else {
		return 1;
	}

	input.close();

	std::vector<std::vector<bool>> intersections; // таблица тривиальных неприводимых пересечений предполны
	uint32_t t, count;
	bool match, new_vec;
	for (uint32_t selection = 1; selection < 262144; selection++) { // генерация всевозможных пересечений п
		if (single_bit(selection)) {
			continue;
		}
		count = 0;
		for (size_t i = 0; i < arr.size(); i++) {
			t = selection;
			match = true;
			for (size_t j = 18; j > 0; j--) {
				if (t & 1) {
					if (arr[i][j - 1] == false) {
						match = false;
						break;
					}
				}
				t = t >> 1;
			}
			if(match == true) {
				count++;
			}
		}
		t = selection;
		std::vector<bool> v_bool(18);
		int_to_vector(t, v_bool);
		if (count == 1) { // если перечение тривиально (нашлась только тождественная функция)
			new_vec = true; // флаг о неприводимости вектора (добавление будет происходить в случае, ес
			for (size_t k = 0; k < intersections.size(); k++) {
				if (contains(intersections[k], v_bool)) { // проверка на
					new_vec = false;
					break;
				}
			}
			if (new_vec) { // добавление
				intersections.push_back(v_bool);
			}
		}
	}
	std::ofstream out;
	out.open("out.txt");
	if (out.is_open()) {
		for (size_t i = 0; i < intersections.size(); i++) {
			if (intersections[i].empty()) {
				continue;
			}
			for (size_t j = 0; j < intersections[i].size(); j++) {
				if (intersections[i][j] == true) {
					out << INTERS_TEXT[j] << ' ';
				}
			}
			out << std::endl;
		}
	} else {
		return 1;
	}
	out.close();
	return 0;
}