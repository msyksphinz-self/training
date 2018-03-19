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

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class bgezal extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "bgezal",
          bgezal.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("offset", BRANCH_OFFSET.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new bgezal(rs, offset);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "bgezal",
          instruction.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("offset", BRANCH_OFFSET.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new instruction(
        new bgezal(
          rs,
          offset
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rs;
  public final IsaPrimitive offset;

  public bgezal(final REG rs, final IsaPrimitive offset) {
    assert REG.INFO.isSupported(rs);
    assert BRANCH_OFFSET.INFO.isSupported(offset);

    this.rs = rs;
    this.offset = offset;

    addArgument("rs", rs);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("bgezal %s, %s", rs.text(vars__), offset.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000001%s10001%s", rs.image(vars__), offset.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if ((rs.access(pe__, vars__).load().compareTo(Data.valueOf(WORD, 0x0)) >= 0)) {
      pe__.GPR.access(31).store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)).add(Data.valueOf(WORD, 0x4)));
      pe__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
      pe__.JMPADDR.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)).add(Data.signExtend(WORD, offset.access(pe__, vars__).load()).shiftLeft(Data.valueOf(WORD, 0x2))));
    }
  }
}