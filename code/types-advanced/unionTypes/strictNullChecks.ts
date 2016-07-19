// --strictNullChecks無しとの比較
// 無しの場合に等しい表現は…
let objA: Date;
// コレです
let objB: Date | null | undefined;

objA = new Date();
// Date単体の型注釈の場合、エラーとなる
// error TS2322: Type 'null' is not assignable to type 'Date'.
// objA = null;

// …しかし、一回anyを経由すればごまかせてしまう 他のサンプルコードでもたまにやってます
objA = null as any;

// objB は null も undefined も許容するため、ゆるゆる
objB = new Date();
objB = null;
objB = void 0; // undefined

// 処理フロー的にundefinedが確定しているのでエラーとなる
// error TS2532: Object is possibly 'undefined'.
// error TS2339: Property 'getTime' does not exist on type 'never'.
// objB.getTime();

export { }
