declare module "jquery";

// こういう定義と同等
declare module "jquery-alt" {
    var _temp: any;
    export = _temp;
}

// ワイルドカードも使える
declare module "json!*";
declare module "sample/*";
