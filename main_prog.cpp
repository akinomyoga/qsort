﻿#include <stdio.h>
#ifdef USE_STDLIB_QSORT
#include <stdlib.h>
#define qsort_selected qsort
#else
#include "qs9e17.h"
#define qsort_selected qsort9e17
#endif
#include <time.h>
#include "random_device.hpp"

#include <array>
#include <functional>
#include <algorithm>
using seed_v_t = std::array<cpprefjp::random_device::result_type, sizeof(std::mt19937) / sizeof(cpprefjp::random_device::result_type)>;
seed_v_t create_seed_v()
{
	cpprefjp::random_device rnd;
	seed_v_t sed_v;
	std::generate(sed_v.begin(), sed_v.end(), std::ref(rnd));
	return sed_v;
}
std::mt19937 create_random_engine()
{
	const auto sed_v = create_seed_v();
	std::seed_seq seq(sed_v.begin(), sed_v.end());
	return std::mt19937(seq);
}
std::mt19937& random_engine() {
	static thread_local std::mt19937 engine = create_random_engine();
	return engine;
}

[[noreturn]] void die(const char *s)
{
	fprintf(stderr, "***** %s *****\n", s); printf("***** %s *****\n", s); exit(1);
}

typedef struct { int key; int data; } el_t;

unsigned int cmp_cnt, ass_cnt;
int g_QS_MID1, g_QS_MID2, g_QS_MID3;

int strcmp0(const char *s1, const char *s2)
{
	int d;
	while ((d = (*s1++ - *s2++)) == 0) if (s1[-1] == 0) return 0;
	return d;
}

int cmp_loop, cmp_val;
int cmpfnc(const void* xp, const void* yp) {
	cmp_cnt++;
	if (cmp_loop) {
		for (int c = cmp_loop; c>0; c--) cmp_val += strcmp0("abc", "def"); 
	}
	return ((const el_t*)(xp))->key - ((const el_t*)(yp))->key;
}

int counter, arr_max, div_val, rec_siz, itarate;
char *vec, *chk;

auto& KEY(int i)
{
	return ((el_t*)(vec + i*rec_siz))->key;
}
auto& DATA(int i)
{
	return ((el_t*)(vec + i * rec_siz))->data;
}
void do_qsort(int do_qs) {
	for (counter = 0; counter<itarate; counter++) {
		/*データを用意する*/
		if (div_val == 0) for (int i = 0; i < arr_max; i++) KEY(i) = 5;         /*一定*/
		if (div_val == -1) for (int i = 0; i < arr_max; i++) KEY(i) = i + 1;       /*昇順*/
		if (div_val == -2) for (int i = 0; i < arr_max; i++) KEY(i) = arr_max - i; /*降順*/
		if (div_val == 1) for (int i = 0; i < arr_max; i++) KEY(i) = std::uniform_int_distribution<>(0, 2147483647)(random_engine());  /*乱数*/
		if (div_val >= 2) for (int i = 0; i < arr_max; i++) KEY(i) = std::uniform_int_distribution<>(0, div_val - 1)(random_engine());
		if (div_val == -3) {
			for (int i = 0; i < arr_max; i++) KEY(i) = i;       /*同値キーがない乱数　入れ替えで*/

			for (int i = 0; i < arr_max; i++) {
				const int x = std::uniform_int_distribution<>(0, arr_max - 1)(random_engine());
				std::swap(KEY(i), KEY(x));
			}
		}

		if (rec_siz >= 8)
			for (int i = 0; i < arr_max; i++) DATA(i) = i;   /*検査のための準備*/

		if (do_qs) qsort_selected((char*)vec, arr_max, rec_siz, cmpfnc);   /*ソートの実行*/

																  /*以下でソートできたことを検査する*/
		for (int i = 1; i < arr_max; i++) {
			if (div_val >= 0 ? KEY(i - 1) > KEY(i) : KEY(i - 1) >= KEY(i)) {
				if (do_qs == 0) continue;
				puts("");
				{
					for (int h = 0; h < arr_max && h < 40; h++) printf(" %d", KEY(h));
					puts(" arr_max error");
				}
				{
					for (int h = 0; h <= i && h < 40; h++) printf(" .");
					puts("← error is here");
				}
				printf("  counter=%d   error i=%d  ", counter, i);
				die("not sorted  do_qsort(1)");
			}
			else {
				if (do_qs == 0) continue;  // do_qsort(1) do_qsort(0) の時間をできるだけ合わせるための処理
			}
			if (rec_siz >= 8) {
				for (int j = 0; j < arr_max; j++) chk[j] = 0;
				for (int j = 0; j < arr_max; j++) chk[DATA(j)] = 123;
				for (int j = 0; j < arr_max; j++) if (chk[j] != 123) die("chk err");
			}
		}
	}
}


int main(int argc, char **argv) {
	clock_t clk_start, clk_end, clk_end2;
	cmp_cnt = ass_cnt = 0;

	if (argc != 9) die("Usage: main.exe div_val arr_max rec_siz itarate MID1 MID2 MID3 cmp");

	div_val = atoi(argv[1]);       /*テストデータの種類を指定する random()%div_val等*/
	arr_max = atoi(argv[2]);       /*要素の個数*/
	rec_siz = atoi(argv[3]);       /*要素の大きさ*/
	itarate = atoi(argv[4]);       /*繰り返し回数*/
	g_QS_MID1 = atoi(argv[5]);     /* n がこれ以下で３点処理を行う qs9の既定値140*/
	g_QS_MID2 = atoi(argv[6]);     /* n がこれ以下で９点処理を行う qs9の既定値900*/
	g_QS_MID3 = atoi(argv[7]);     /* 使用しない */
	cmp_loop = atoi(argv[8]);     /*比較関数の重たさを調整する*/

	fprintf(stderr, "\n%-8s d=%d e=%d s=%d %dM R%d ",
		argv[0] + 2, div_val, arr_max, rec_siz, arr_max*rec_siz / 1000000, itarate);
	fprintf(stderr, "%c=%d:%d:%d:%d: ", (sizeof(char*) == 8 ? 'M' : 'm'), g_QS_MID1, g_QS_MID2, g_QS_MID3, cmp_loop);

	printf("%-9s d=%d e=%d s=%d %d R%d ",
		argv[0] + 2, div_val, arr_max, rec_siz, arr_max*rec_siz / 1000000, itarate);
	printf("%c%3d:%3d:%d:%d:", (sizeof(char*) == 8 ? 'M' : 'm'), g_QS_MID1, g_QS_MID2, g_QS_MID3, cmp_loop);
	fflush(stdout);

	if (rec_siz < 4 || 20000 < rec_siz) die("本プログラムでは「要素のバイトサイズは４以上&２万以下」");
	if (rec_siz % 4) die("本プログラムでは「要素のバイトサイズは４の倍数」を仮定");
	if ((vec = (char*)malloc((arr_max + 5)*rec_siz)) == NULL) die("vec NULL");
	if ((chk = (char*)malloc(arr_max)) == NULL) die("chk NULL");

	clk_start = clock();
	do_qsort(1);
	clk_end = clock();
	do_qsort(0);
	clk_end2 = clock();

	{
		unsigned int cmp_av = cmp_cnt / itarate;
		unsigned int ass_av = ass_cnt / itarate;
		double sum_av = (double)cmp_av + ass_av;
		double etime = (double)((clk_end - clk_start) - (clk_end2 - clk_end)) / CLOCKS_PER_SEC;
		fprintf(stderr, "   %4.0f ", etime / itarate * 100000);
		printf(" c=%-6u %10u a=%-6u %10u i=%1.0f T=%1.2f %4.0f \n", cmp_av, cmp_cnt, ass_av, ass_cnt, sum_av, etime, etime / itarate * 100000);
	}

	fflush(stdout);
	return 0;
}