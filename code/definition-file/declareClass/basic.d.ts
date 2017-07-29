// A. クラスを定義する
declare class TestA {
}

// B. クラスの分解定義 変数 + インタフェース2つ
declare let TestB: TestBConstructor;
interface TestBConstructor {
	new(): TestB;
}
interface TestB {
}
