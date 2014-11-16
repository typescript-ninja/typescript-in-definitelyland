module.exports = function (grunt) {
    function generateTypeScriptCompileSettings() {
        var srcFileList = grunt.file.expand([
            'code/**/*.ts',
            '!code/**/*-invalid.ts', // コンパイルが通らないコード
            '!code/**/*-1.3.0.ts' // 1.3.0 限定コード
        ]);
        var tasks = {};
        srcFileList.forEach(function (file) {
            var task = tasks[file] = {
                src: file,
                options: {
                    compiler: './typescript/bin/tsc',
                    target: 'es6',
                    module: 'commonjs',
                    noImplicitAny: true,
                    sourceMap: false,
                    declaration: false
                }
            };
            switch (file) {
                case "notExists/hogehoge.ts":
                    task.options.declaration = true;
                    break;
                default:
                    break;
            }
            switch (file) {
                case "notExists/fugafuga.ts":
                    task.options.module = "amd";
                    break;
                default:
                    break;
            }
        });
        return tasks;
    }

    grunt.initConfig({
        ts: generateTypeScriptCompileSettings(),
        tslint: {
            options: {
                configuration: grunt.file.readJSON("tslint.json")
            },
            files: {
                src: [
                    'code/**/*.ts'
                ]
            }
        },
        'typescript-formatter': {
            options: {
                configuration: grunt.file.readJSON("tsfmt.json")
            },
            files: {
                src: [
                    'code/**/*.ts'
                ],
                dest: "code"
            }
        },
        dtsm: {
            client: {
                options: {
                    // optional: specify config file
                    confog: './dtsm.json'
                }
            }
        },
        clean: {
            ts: {
                src: [
                    'code/**/*.js',
                    'code/**/*.js.map'
                ]
            },
            review: {
                src: [
                    'articles/c87-typescript-pdf/',
                    'articles/*.pdf'
                ]
            }
        },
        exec: {
            preprocess: {
                cwd: "./articles",
                cmd: function () {
                    var command = "review-preproc";
                    var files = [
                        "articles"
                    ];
                    var exec = command + " -r --tabwidth=2 *.re";
                    console.log(exec);
                    return exec;
                }
            },
            compile2text: {
                cwd: "./articles",
                cmd: function () {
                    return "review-compile --all --target=text";
                }
            },
            compile2html: {
                cwd: "./articles",
                cmd: function () {
                    return "review-compile --all --target=html";
                }
            },
            compile2latex: {
                cwd: "./articles",
                cmd: function () {
                    return "review-compile --all --target=latex";
                }
            },
            compile2idgxml: {
                cwd: "./articles",
                cmd: function () {
                    return "review-compile --all --target=idgxml";
                }
            },
            compile2pdf: {
                cwd: "./articles",
                cmd: function () {
                    return "review-pdfmaker config.yml";
                }
            },
            compile2epub: {
                cwd: "./articles",
                cmd: function () {
                    return "review-epubmaker config.yml";
                }
            },
            checkWithTsc: {
                cwd: "./articles",
                cmd: function () {
                    return "review-compile --all --target=html tsc-check";
                }
            }
        }
    });

    function generateTask(target) {
        return ['clean', 'typescript-formatter', 'ts', 'exec:preprocess', 'exec:compile2' + target];
    }

    grunt.registerTask(
        'default',
        "原稿をコンパイルしてPDFファイルにする",
        "pdf");

    grunt.registerTask(
        'check',
        "原稿に含まれるTypeScriptコードをtscに食わせてチェックする",
        "exec:checkWithTsc");

    grunt.registerTask(
        'text',
        "原稿をコンパイルしてTextファイルにする",
        generateTask("text"));

    grunt.registerTask(
        'html',
        "原稿をコンパイルしてHTMLファイルにする",
        generateTask("html"));

    grunt.registerTask(
        'idgxml',
        "原稿をコンパイルしてInDesign用XMLファイルにする",
        generateTask("idgxml"));

    grunt.registerTask(
        'pdf',
        "原稿をコンパイルしてpdfファイルにする",
        generateTask("pdf"));

    grunt.registerTask(
        'epub',
        "原稿をコンパイルしてepubファイルにする",
        generateTask("epub"));

    require('load-grunt-tasks')(grunt);
};
