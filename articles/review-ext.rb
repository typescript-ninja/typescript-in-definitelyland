# encoding: utf-8

require 'pp'
require "tmpdir"
require "open3"

module ReVIEW
  class HTMLBuilder
    # リンクマーク出したい
    def headline(level, label, caption)
      prefix, anchor = headline_prefix(level)
      puts '' if level > 1
      a_id = ""
      unless anchor.nil?
        a_id = %Q[<a id="h#{anchor}" href="#h#{anchor}" aria-hidden="true"><span class="octicon octicon-link">&sect;</span></a>]
      end
      if caption.empty?
        puts a_id unless label.nil?
      else
        if label.nil?
          puts %Q[<h#{level}>#{a_id}#{prefix}#{compile_inline(caption)}</h#{level}>]
        else
          puts %Q[<h#{level} id="#{normalize_id(label)}">#{a_id}#{prefix}#{compile_inline(caption)}</h#{level}>]
        end
      end
    end
  end
end
