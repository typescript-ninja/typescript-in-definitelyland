// A. 普通にクラスを定義する
declare class TestA { }

// B. クラスの分解定義 変数 + インタフェース2つ
declare var TestB: TestBStatic;
interface TestBStatic {
    new (): TestB;
}
interface TestB {
}
