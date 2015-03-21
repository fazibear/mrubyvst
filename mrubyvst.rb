# PROGRAMS_COUNT
# PARAMETERS_COUNT

# effect_name() -> String
# product()     -> String
# vendor()      -> String
# version()     -> Fixnum

# change_program(index) -> Nil
# program_name(index)   -> String

# set_paramter(index, value)     -> Nil
# parameter_name(index)          -> String
# parameter_value(index)         -> Float
# parameter_display_value(index) -> String
# parameter_label(index)         -> String
# process(data)                  -> Array

class MRubyVST
  attr_reader :vendor, :product, :effect_name, :version

  def initialize
    @vendor = 'Mruby'
    @product = 'MrubyVST'
    @effect_name = 'MRubyEffects'
    @version = 0
    @programs = [
      EmptyVST,
      GainVST,
      PanVST,
      WidnerVST,
    ]
  end

  def change_program(index)
    @program = @programs[index].new if @programs[index]
  end

  def program_name(index)
    @programs[index].to_s
  end

  def set_parameter(index, value)
    @program.set_parameter(index, value) if @program
  end

  def parameter_name(index)
    @program.parameter_name(index) if @program
  end

  def parameter_value(index)
    @program.parameter_value(index) if @program
  end

  def parameter_display_value(index)
    @program.parameter_display_value(index) if @program
  end

  def parameter_label(index)
    @program.parameter_label(index) if @program
  end

  def process(data)
    @program.process(data) if @program
  end

  def log(str)
    io = File.open('/tmp/mrubyvst.log', 'a')
    io.write(str + "\n")
    io.close
  end
end

class EmptyVST
  def set_parameter(index, value)
  end

  def parameter_name(index)
    'empty'
  end

  def parameter_value(index)
    0.0
  end

  def parameter_display_value(index)
    '0'
  end

  def parameter_label(index)
  end

  def process(data)
    data
  end
end

class GainVST
  def initialize
    @gain = 1.0
  end

  def set_parameter(index, value)
    @gain = value if index = 0
  end

  def parameter_name(index)
    index == 0 ? 'Gain' : 'empty'
  end

  def parameter_value(index)
    @gain if index == 0
  end

  def parameter_display_value(index)
    @gain.to_s if index == 0
  end

  def parameter_label(index)
    'dB' if index == 0
  end

  def process(data)
    data[0].map!{ |left | left * @gain}
    data[1].map!{ |right| right * @gain }
    data
  end
end

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

class WidnerVST
  def initialize
    @width   = 0
    @mid     = 1
    @side    = 1
  end

  def set_parameter(index, value)
    @width = value if index == 0
  end

  def parameter_name(index)
    index == 0 ? "Width" : "empty"
  end

  def parameter_value(index)
    @pan if index == 0
  end

  def parameter_display_value(index)
    (@width * 10).to_i.to_s if index == 0
  end

  def parameter_label(index)
  end

  def process(data)
    length = data[0].length
    length.times do |t|
      data[0][t], data[1][t] = process_lr(data[0][t], data[1][t])
    end
    data
  end

  def process_lr(l,r)
    mid  = 0.5 * (l + r)
    side = 0.5 * (r - l) * @width
    [mid - side, mid + side]
  end
end
