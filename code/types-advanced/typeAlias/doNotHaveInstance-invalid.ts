// 型の別名を作るだけで何かの値を作るわけではない…！
type StringArray = Array<string>;

// なのでこういうことはできない
// error TS2304: Cannot find name 'StringArray'.
let strArray = new StringArray();
