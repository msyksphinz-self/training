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

public final class popA extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "popA",
          popA.class,
          null
          );
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      return new popA();
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "popA",
          instruction.class,
          null
          );
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      return new instruction(
        new popA()
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public popA() {
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("popA");
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("01100001");
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    pe__.AH.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xf))).load());
    pe__.AL.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xe))).load());
    pe__.CH.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xd))).load());
    pe__.CL.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xc))).load());
    pe__.DH.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xb))).load());
    pe__.DL.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0xa))).load());
    pe__.BH.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x9))).load());
    pe__.BL.access().store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x8))).load());
    pe__.BP.access().bitField(15, 8).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x5))).load());
    pe__.BP.access().bitField(7, 0).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x4))).load());
    pe__.SI.access().bitField(15, 8).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x3))).load());
    pe__.SI.access().bitField(7, 0).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x2))).load());
    pe__.DI.access().bitField(15, 8).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load()).add(Data.valueOf(WORD, 0x1))).load());
    pe__.DI.access().bitField(7, 0).store(pe__.MEM.access(pe__.SS.access().load().add(pe__.SP.access().load())).load());
    pe__.SP.access().store(pe__.SP.access().load().add(Data.valueOf(WORD, 0x10)));
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x1));
  }
}