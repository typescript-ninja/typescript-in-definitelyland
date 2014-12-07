var str = "str";
// anyを経由しない場合、整合性の無い型アサーションは成功しない！安全！
// error TS2352: Neither type 'string' nor type 'number' is assignable to the other.
var num: number = <number> str;
