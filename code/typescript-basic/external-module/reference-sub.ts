// Node.jsだと sub/ で自動的に sub/index と同じ扱いになるのだが…
// import sub = require("sub/");
// TypeScript上では index が省略不可になっている
import sub = require("./sub/index");

console.log(sub.hello());
