def _dot_var (v, verbose=false)

  name = v.name == nil ? '' : v.name
  if verbose and v.data != nil then
    if v.name != nil then
      name += ': '
    end
    name += v.shape.to_s + ' ' + v.dtype.to_s
  end
  return sprintf("%s [label=\"%s\", color=orange, style=filled]\n",
                 v.object_id, name)
end

def _dot_func(f)

  txt = sprintf("%s [label=\"%s\", color=lightblue, style=filled, shape=box]\n",
                f.object_id, f.class.to_s)
  f.inputs.each{|x|
    txt += sprintf("%s -> %s\n", x.object_id, f.object_id)
  }
  f.outputs.each{|y|
    txt += sprintf("%s -> %s\n", f.object_id, y.__getobj__.object_id)
  }
  return txt
end


def get_dot_graph(output, verbose=true)
  funcs = Array.new()
  seen_set = Set.new()
  txt = ""

  def add_func(f, funcs, seen_set)
    if not seen_set.include?(f) then
      funcs.push(f)
      seen_set.add(f)
    end
  end

  add_func(output.creator, funcs, seen_set)
  txt += _dot_var(output, verbose)

  while not funcs.empty? do
    func = funcs.pop
    txt += _dot_func(func)
    func.inputs.each{|x|
      txt += _dot_var(x, verbose)

      if x.creator != nil then
        add_func(x.creator, funcs, seen_set)
      end
    }
  end

  return "digraph g {\n" + txt + "}"
end


def plot_dot_graph(output, verbose=true, to_file='graph.png')
  dot_graph = get_dot_graph(output, verbose)

  f = File.new("tmp_graph.dot", "w")
  f.write(dot_graph)

  cmd = "dot " + "tmp_graph.dot" + " -T png -o " + to_file
  %x[#{cmd}]
end
