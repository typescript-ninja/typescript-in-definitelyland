let str = "str";
// anyを経由しない場合、整合性の無い型アサーションは成功しない！安全！
// error TS2352: Type 'string' cannot be converted to type 'number'.
let num: number = str as number;
