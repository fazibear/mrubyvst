class PanVST
  def initialize
    @pan   = 0.5
    @left  = 1
    @right = 1
  end

  def set_parameter(index, value)
    if index == 0
      @pan = value

      rad = (45 - (@pan * 45 * 2)) * Math::PI / 180

      @left = Math.cos(rad) + Math.sin(rad)
      @right = Math.cos(rad) - Math.sin(rad)
    end
  end

  def parameter_name(index)
    index == 0 ? "Pan" : "empty"
  end

  def parameter_value(index)
    @pan if index == 0
  end

  def parameter_display_value(index)
    if index == 0
      case @pan
      when 0.5 then "center"
      when 0..0.5 then "#{(100 - @pan*200).to_i}% L"
      when 0.5..1 then "#{(@pan*200 - 100).to_i}% R"
      end
    end
  end

  def parameter_label(index)
  end

  def process(data)
    data[0].map!{ |left | left  * @left }
    data[1].map!{ |right| right * @right }
    data
  end
end

PanVST.new
