/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class pushA extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "pushA",
          pushA.class,
          null
          );
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      return new pushA();
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "pushA",
          instruction.class,
          null
          );
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      return new instruction(
        new pushA()
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public pushA() {
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("pushA");
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("01100000");
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    pe__.SP.access().store(pe__.SP.access().load().subtract(Data.valueOf(WORD, 0x10)));
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xf))).store(pe__.AH.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xe))).store(pe__.AL.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xd))).store(pe__.CH.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xc))).store(pe__.CL.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xb))).store(pe__.DH.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xa))).store(pe__.DL.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x9))).store(pe__.BH.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x8))).store(pe__.BL.access().load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x7))).store(pe__.SP.access().bitField(15, 8).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x6))).store(pe__.SP.access().bitField(7, 0).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x5))).store(pe__.BP.access().bitField(15, 8).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x4))).store(pe__.BP.access().bitField(7, 0).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x3))).store(pe__.SI.access().bitField(15, 8).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x2))).store(pe__.SI.access().bitField(7, 0).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x1))).store(pe__.DI.access().bitField(15, 8).load());
    pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x0))).store(pe__.DI.access().bitField(7, 0).load());
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x1));
  }
}