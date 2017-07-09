// 2つのコンストラクタとそれぞれの返り値の型
// コンストラクタの片方は ...args: any[] を引数に取る
type ConstructorA<T, U> = { new(s: string): T } & { new(...args: any[]): U };

// 1つのコンストラクタと返り値は2つの型の交差型
type ConstructorB<T, U> = { new(s: string): T & U };

// 2つの定義には互換性がある！
const A: ConstructorA<Date, RegExp> = null as any;
const B: ConstructorB<Date, RegExp> = A;
const A2: ConstructorA<Date, RegExp> = B;
