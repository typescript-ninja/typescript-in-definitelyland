let clazz: {
	new (): any;
};

// 当てはまる値はこんな感じ
class Sample {
}
clazz = Sample;
let obj = new clazz();

// クラス式だとこんな感じ
clazz = class { };
obj = new clazz();

export { }
