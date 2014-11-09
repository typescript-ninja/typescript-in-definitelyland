# encoding: utf-8

require 'pp'
require "tmpdir"
require "open3"

module ReVIEW
  module Book
    class ListIndex
      def ListIndex.item_type
        '(list|listnum|tslist|jslist)'
      end
    end
  end

  class Compiler
    defblock :tslist, 2..4
    defblock :emtslist, 0..3
    defblock :jslist, 2..3
    defblock :emjslist, 0..2
  end

  class Builder
    def check_format(file, id, scripts)
      o, e, s = Open3.capture3("../node_modules/.bin/tsfmt --no-tslint --no-editorconfig #{file.path}")
      result = o.gsub(/\t/, "  ").rstrip != scripts.rstrip
      if result
        error "#{id} is not valid code style typescript code\nexpected\n#{o.gsub(/\t/, "  ").rstrip}\n\n"
      end
      return result
    end

    def check_ts_syntax(file, id, tsc_options, fail)
      o, e, s = Open3.capture3("../node_modules/.bin/tsc #{tsc_options} #{file.path}")
      if !s.success? and !fail
        # error "#{id} is invalid typescript code, compile with '#{tsc_options}'\n#{scripts}\n\n#{e}\n\n"
        error "#{id} is invalid typescript code, compile with '#{tsc_options}'"
      elsif s.success? and fail
        # error "#{id} is unexpected valid typescript code, compile with '#{tsc_options}'\n#{scripts}\n\n#{e}\n\n"
        error "#{id} is unexpected valid typescript code, compile with '#{tsc_options}'"
      end
    end

    def typescript_check(id, scripts, tsc_options = "", check_options = "")
      if !ARGV.include?("tsc-check")
        return
      end
      # pp "check #{@chapter.name}.#{id}"
      Dir.mktmpdir("typescript-book") {|dir|
        FileUtils.copy("../tsfmt.json", dir)
        file = open("#{dir}/tmp.ts", "w")
        file.puts scripts
        file.close

        check_ts_syntax(file, id, tsc_options, check_options.index("fail"))

        if !check_options.index("nofmt")
          check_format(file, id, scripts)
        end
      }
    end

    def javascript_check(id, scripts, check_options = "")
      if !ARGV.include?("tsc-check")
        return
      end
      # pp "check #{@chapter.name}.#{id}"
      Dir.mktmpdir("typescript-book") {|dir|
        FileUtils.copy("../tsfmt.json", dir)
        file = open("#{dir}/tmp.ts", "w")
        file.puts scripts
        file.close

        if !check_options.index("nofmt")
          check_format(file, id, scripts)
        end
      }
    end

    def tslist(lines, id, caption, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check id, lines.join("\n"), tsc_options, check_options
      list(lines, id, caption)
    end

    def emtslist(lines, caption = nil, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check "", lines.join("\n"), tsc_options, check_options
      emlist(lines, caption)
    end

    def jslist(lines, id, caption, check_options = "")
      javascript_check id, lines.join("\n"), check_options
      list(lines, id, caption)
    end

    def emjslist(lines, caption = nil, check_options = "")
      javascript_check "", lines.join("\n"), check_options
      emlist(lines, caption)
    end
  end

  class HTMLBuilder
    def tslist(lines, id, caption, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check id, unescape_html(lines.join("\n")), tsc_options, check_options
      list(lines, id, caption)
    end

    def emtslist(lines, caption = nil, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check "", unescape_html(lines.join("\n")), tsc_options, check_options
      emlist(lines, caption)
    end

    def jslist(lines, id, caption, check_options = "")
      javascript_check id, unescape_html(lines.join("\n")), check_options
      list(lines, id, caption)
    end

    def emjslist(lines, caption = nil, check_options = "")
      javascript_check "", unescape_html(lines.join("\n")), check_options
      emlist(lines, caption)
    end
  end

  class LATEXBuilder
    def tslist(lines, id, caption, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check id, unescape_latex(lines.join("\n")), tsc_options, check_options
      list(lines, id, caption)
    end

    def emtslist(lines, caption = nil, tsc_options = "--module commonjs --target es5", check_options = "")
      typescript_check "", unescape_latex(lines.join("\n")), tsc_options, check_options
      emlist(lines, caption)
    end

    def jslist(lines, id, caption, check_options = "")
      javascript_check id, unescape_latex(lines.join("\n")), check_options
      list(lines, id, caption)
    end

    def emjslist(lines, caption = nil, check_options = "")
      javascript_check "", unescape_latex(lines.join("\n")), check_options
      emlist(lines, caption)
    end
  end
end
