// 同じ実装に対して、どの型定義が一番便利かな？

// getのとき setのとき 仕様が違うことがよく分かる
declare function valueA(value: any): void;
declare function valueA(): any;

// setのときも値が取れるっぽい？
declare function valueB(value?: any): any;

// 詳細が不明だ…！
declare var valueC: Function;
