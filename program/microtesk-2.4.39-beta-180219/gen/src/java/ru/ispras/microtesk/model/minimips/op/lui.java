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

package ru.ispras.microtesk.model.minimips.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;
import ru.ispras.microtesk.model.minimips.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class lui extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "lui",
          lui.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("imm", SHORT);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final Immediate imm = (Immediate) getArgument("imm", args);

      return new lui(rt, imm);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "lui",
          instruction.class,
          null
          );
      addArgument("rt", REG.INFO);
      addArgument("imm", SHORT);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rt = (REG) getArgument("rt", args);
      final Immediate imm = (Immediate) getArgument("imm", args);

      return new instruction(
        new lui(
          rt,
          imm
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rt;
  public final Immediate imm;

  public lui(final REG rt, final Immediate imm) {
    assert REG.INFO.isSupported(rt);
    assert SHORT.equals(imm.getType());

    this.rt = rt;
    this.imm = imm;

    addArgument("rt", rt);
    addArgument("imm", imm);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("lui %s, 0x%x", rt.text(vars__), imm.access().load().bigIntegerValue(false));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("00111100000%s%s", rt.image(vars__), imm.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rt.access(pe__, vars__).store(Data.signExtend(WORD, imm.access().load()).shiftLeft(Data.valueOf(WORD, 0x10)));
  }
}