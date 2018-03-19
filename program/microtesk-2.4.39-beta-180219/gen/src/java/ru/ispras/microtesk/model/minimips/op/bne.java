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

public final class bne extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "bne",
          bne.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("offset", BRANCH_OFFSET.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG rt = (REG) getArgument("rt", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new bne(rs, rt, offset);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "bne",
          instruction.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("offset", BRANCH_OFFSET.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG rt = (REG) getArgument("rt", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new instruction(
        new bne(
          rs,
          rt,
          offset
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rs;
  public final REG rt;
  public final IsaPrimitive offset;

  public bne(final REG rs, final REG rt, final IsaPrimitive offset) {
    assert REG.INFO.isSupported(rs);
    assert REG.INFO.isSupported(rt);
    assert BRANCH_OFFSET.INFO.isSupported(offset);

    this.rs = rs;
    this.rt = rt;
    this.offset = offset;

    addArgument("rs", rs);
    addArgument("rt", rt);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("bne %s, %s, %s", rs.text(vars__), rt.text(vars__), offset.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000101%s%s%s", rs.image(vars__), rt.image(vars__), offset.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (!rs.access(pe__, vars__).load().equals(rt.access(pe__, vars__).load())) {
      pe__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
      pe__.JMPADDR.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)).add(Data.signExtend(WORD, offset.access(pe__, vars__).load()).shiftLeft(Data.valueOf(WORD, 0x2))));
    }
  }
}