// 同じ実装に対して、どの型定義が一番便利かな？
// 1関数でget, set両方の役目を果たす場合…

// getのとき setのとき 仕様が違うことがよく分かる
declare function valueA(value: any): void;
declare function valueA(): any;

// setのときも値が取れる気がする…？
declare function valueB(value?: any): any;

// 詳細が不明だ…！
declare let valueC: Function;
