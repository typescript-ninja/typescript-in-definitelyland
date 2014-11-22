// 同じ実装に対して、どの型定義が一番便利かな？

// getの時 setの時 仕様が違うことがよくわかる
declare function valueA(value: any): void;
declare function valueA(): any;

// setの時も値が取れるっぽい？
declare function valueB(value?: any): any;

// 詳細が不明だ…！
declare var valueC: Function;
