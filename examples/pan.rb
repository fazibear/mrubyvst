class VST
  attr_reader :vendor, :product, :version

  def initialize
    @vendor  = 'MRuby powered plugin'
    @product = 'Pan Plugin'
    @name    = 'Pan Vst'
    @version = 0
    @pan     = 0.5
    @left    = 1
    @right   = 1
  end

  def parameter_set(index, value)
    if index == 0
      @pan = value

      rad = (45 - (@pan * 45 * 2)) * Math::PI / 180

      @left = Math.cos(rad) + Math.sin(rad)
      @right = Math.cos(rad) - Math.sin(rad)
    end
  end

  def parameter_get(index)
    @pan if index == 0
  end

  def parameter_name(index)
    if index == 0
      "Pan"
    else
      "empty"
    end
  end

  def parameter_display(index)
    if index == 0
      case @pan
      when 0.5 then "center"
      when 0..0.5 then "#{(100 - @pan*200).to_i}% L"
      when 0.5..1 then "#{(@pan*200 - 100).to_i}% R"
      end
    else
      ''
    end
  end

  def parameter_label(index)
    if index == 0
      ''
    else
      ''
    end
  end

  def process_replacing(data)
    data[0].map!{ |left | left  * @left }
    data[1].map!{ |right| right * @right }
    data
  end

end
