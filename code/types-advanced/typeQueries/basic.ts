class Sample {
	str: string;
}

// この書き方だとSampleのインスタンスになる Sampleクラスそのものではない
let obj: Sample;
// Sample自体の型をコピー！ つまりこれはSampleクラスそのものです
let clazz: typeof Sample;

// それぞれに当てはまる値は次のとおり なるほど
obj = new Sample();
clazz = Sample;

obj = new clazz();

// clazz を頑張って手で書くと次に等しい
let alterClazz: {
	new (): { str: string; };
};
alterClazz = clazz;
clazz = alterClazz;

export { }
