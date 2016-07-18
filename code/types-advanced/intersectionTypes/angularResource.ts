// intersection typesを使わない書き方
declare namespace angular.resource1 {
	interface ResourceProvider {
		create<T extends Resource<any>>(): T;
	}

	interface Resource<T> {
		$insert(): T;
	}
	let $resource: ResourceProvider;
}
// 上の定義を使ってみる
namespace sample1 {
	interface Sample {
		str: string;
	}
	// SampleResourceという型を1つ無駄に作らねばならぬ
	// なぜこれで動くのか、トリックがわかるだろうか？
	interface SampleResource extends Sample, angular.resource1.Resource<Sample> { }

	let $obj = angular.resource1.$resource.create<SampleResource>();
	$obj.str = "test";
	let obj = $obj.$insert();
	console.log(obj.str);
}

// intersection typesを使った書き方
declare namespace angular.resource2 {
	interface ResourceProvider {
		create<T>(): T & Resource<T>;
	}

	interface Resource<T> {
		$insert(): T;
	}
	let $resource: ResourceProvider;
}
// 上の定義を使ってみる
namespace sample2 {
	interface Sample {
		str: string;
	}

	// 超簡単…！！
	let $obj = angular.resource2.$resource.create<Sample>();
	$obj.str = "test";
	let obj = $obj.$insert();
	console.log(obj.str);
}

export { sample1, sample2 }
