class Sample {
	str: string;
}

// この書き方だとSampleのインスタンスになる Sampleクラスそのものではない
var obj: Sample;
// Sample自体の型をコピー！ つまりこれはSampleクラスそのものだ！
var clazz: typeof Sample;

// それぞれに当てはまる値は以下の通り なるほどな！
obj = new Sample();
clazz = Sample;

obj = new clazz();

// clazz を頑張って手で書くと以下に等しい
var alterClazz: {
	new (): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;
