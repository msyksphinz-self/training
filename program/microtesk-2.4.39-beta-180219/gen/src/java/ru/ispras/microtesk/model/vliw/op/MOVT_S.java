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

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.vliw.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class MOVT_S extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "MOVT_S",
          MOVT_S.class,
          null
          );
      addArgument("fd", F.INFO);
      addArgument("fs", F.INFO);
      addArgument("cc", COND_CODE);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final F fd = (F) getArgument("fd", args);
      final F fs = (F) getArgument("fs", args);
      final Immediate cc = (Immediate) getArgument("cc", args);

      return new MOVT_S(fd, fs, cc);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final F fd;
  public final F fs;
  public final Immediate cc;

  public MOVT_S(final F fd, final F fs, final Immediate cc) {
    assert F.INFO.isSupported(fd);
    assert F.INFO.isSupported(fs);
    assert COND_CODE.equals(cc.getType());

    this.fd = fd;
    this.fs = fs;
    this.cc = cc;

    addArgument("fd", fd);
    addArgument("fs", fs);
    addArgument("cc", cc);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("MOVT_S %s, %s", fd.text(vars__), fs.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000110000%s01%s%s010001", cc.access().load().toString(), fs.image(vars__), fd.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (pe__.FCCR.access().bitField(cc.access().load(), cc.access().load()).load().equals(Data.valueOf(Type.INT(1), 0x1))) {
      fd.access(pe__, vars__).store(fs.access(pe__, vars__).load());
    }
  }
}