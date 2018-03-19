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
import ru.ispras.microtesk.model.x86.mode.*;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class push_r16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "push_r16",
          push_r16.class,
          null
          );
      addArgument("src", R16.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 src = (R16) getArgument("src", args);

      return new push_r16(src);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "push_r16",
          instruction.class,
          null
          );
      addArgument("src", R16.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 src = (R16) getArgument("src", args);

      return new instruction(
        new push_r16(
          src
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R16 src;

  public push_r16(final R16 src) {
    assert R16.INFO.isSupported(src);

    this.src = src;

    addArgument("src", src);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("push %s", src.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("1010%s", src.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    vars__.base.access().store(Data.zeroExtend(addR, pe__.SS.access().load()).shiftLeft(Data.valueOf(addR, 0x4)));
    pe__.SP.access().store(pe__.SP.access().load().subtract(Data.valueOf(WORD, 0x2)));
    pe__.MEM.access(vars__.base.access().load().add(Data.signExtend(addR, Data.cast(SWORD, pe__.SP.access().load())))).store(src.access(pe__, vars__).bitField(7, 0).load());
    pe__.MEM.access(vars__.base.access().load().add(Data.signExtend(addR, Data.cast(SWORD, pe__.SP.access().load().add(Data.valueOf(WORD, 0x1)))))).store(src.access(pe__, vars__).bitField(15, 8).load());
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x1));
  }
}